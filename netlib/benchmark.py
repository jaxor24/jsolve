import logging
import subprocess
from pathlib import Path

logger = logging.getLogger(__name__)
logging.basicConfig(format="[%(levelname)s] %(message)s", level=logging.DEBUG)

logger.info("Starting...")

mps_directory = "netlib/mps"

all_mps_files = Path(mps_directory).glob("*.mps")
solver = Path("C:\\repos\\jsolve\\out\\build\\windows-release\\app\\jsolver_app.exe")

for i, file in enumerate(all_mps_files):
    logger.info(f"{i} {file.name}")
    subprocess.run([solver, "--mps", file, "--log", "info"])

logger.info("Done.")
