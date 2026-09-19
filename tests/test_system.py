"""End-to-end tests run in temporary directories."""

from pathlib import Path
import shutil
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]

subprocess.run(
    ["make", "-C", str(root)],
    check=True
)

exe = root / "campus_reservations"
checks = 0


def run(home, commands):
    result = subprocess.run(
        [str(exe)],
        input=commands,
        text=True,
        capture_output=True,
        cwd=home,
        timeout=10
    )

    assert result.returncode == 0, result.stderr
    return result.stdout


def check(condition):
    global checks
    assert condition
    checks += 1


def create(
    student_id,
    resource="R101",
    date="2026-10-01",
    name="Test Student"
):
    return (
        f"2\n{student_id}\n{name}\n"
        f"{resource}\n{date}\n"
    )


with tempfile.TemporaryDirectory() as temp:
    home = Path(temp)
    (home / "data").mkdir()

    shutil.copy(
        root / "data/resources.txt",
        home / "data/resources.txt"
    )

    # Different dates can be reserved independently.
    # Cancelling ID 1 promotes the first eligible waiter.
    out = run(
        home,
        create(11)
        + create(22)
        + create(33)
        + create(44, date="2026-10-02")
        + create(55, date="2026-10-02")
        + create(66)
        + "3\n1\n5\n8\n9\n"
    )

    active = (
        home / "data/reservations.txt"
    ).read_text()

    waiting = (
        home / "data/waiting.txt"
    ).read_text()

    check(
        "2,44,Test Student,R101,2026-10-02"
        in active
    )

    check(
        "3,22,Test Student,R101,2026-10-01"
        in active
    )

    check("Cannot undo" in out)

    check(
        waiting.splitlines() == [
            "R101,33,Test Student,2026-10-01",
            "R101,55,Test Student,2026-10-02",
            "R101,66,Test Student,2026-10-01"
        ]
    )

    # Remove a waiter, cancel, and successfully undo.
    out = run(
        home,
        "10\n55\nR101\n2026-10-02\n"
        "3\n2\n5\n8\n9\n"
    )

    check("Restored reservation 2." in out)

    check(
        "55" not in (
            home / "data/waiting.txt"
        ).read_text()
    )

    # Duplicate requests and invalid input.
    out = run(
        home,
        create(22)
        + create(-1)
        + create(1, resource="BAD")
        + create(1, date="2026-02-29")
        + create(1, name="Bad,Name")
        + "nonsense\n1abc\n"
        + "1\n3\n2026-10-01\n9\n"
    )

    check("already have" in out)
    check(out.count("Invalid request:") == 4)
    check(out.count("Invalid number.") == 2)
    check("Availability for 2026-10-01" in out)

    # FIFO promotion survives saving and reloading.
    run(home, "3\n3\n9\n")

    check(
        "4,33,Test Student,R101,2026-10-01"
        in (
            home / "data/reservations.txt"
        ).read_text()
    )

    # End-of-input at the menu saves and exits.
    out = run(home, "")
    check("All data saved." in out)

    # End-of-input inside a prompt also exits.
    out = run(home, "2\n")
    check("All data saved." in out)

    # Invalid saved IDs and conflicting bookings are skipped.
    with (
        home / "data/reservations.txt"
    ).open("a") as output:
        output.write(
            "12abc,8,Bad,R101,2026-11-01\n"
            "99,8,Bad,R101,2026-10-01\n"
        )

    run(home, "9\n")

    check(
        "Bad" not in (
            home / "data/reservations.txt"
        ).read_text()
    )

    # Undo restores the most recent cancellation first.
    out = run(
        home,
        "3\n4\n3\n2\n5\n8\n9\n"
    )

    check("Restored reservation 2." in out)

    check(
        "5,66,Test Student,R101,2026-10-01"
        in (
            home / "data/reservations.txt"
        ).read_text()
    )


# Missing and empty inventory must fail clearly.
with tempfile.TemporaryDirectory() as temp:
    result = subprocess.run(
        [str(exe)],
        cwd=temp,
        input="",
        text=True,
        capture_output=True
    )

    check(
        result.returncode == 1
        and "Fatal" in result.stderr
    )

    (Path(temp) / "data").mkdir()
    (
        Path(temp) / "data/resources.txt"
    ).write_text("")

    result = subprocess.run(
        [str(exe)],
        cwd=temp,
        input="",
        text=True,
        capture_output=True
    )

    check(result.returncode == 1)

print(f"Passed {checks} end-to-end checks.")
