import logging
import logging.handlers
import subprocess
import multiprocessing as mp
from functools import partial

from pathlib import Path

from random import choice, random

LOGGER = "LOGGER"

def listener_configurer():
    root = logging.getLogger()
    h = logging.FileHandler('benchmark.log', mode='w')
    f = logging.Formatter('[%(asctime)s][%(processName)-13s][%(levelname)-8s] %(message)s')
    h.setFormatter(f)
    root.addHandler(h)


def listener_process(queue, configurer):
    configurer()
    while True:
        try:
            record = queue.get()
            if record is None:
                break
            logger = logging.getLogger(record.name)
            logger.handle(record)
        except Exception:
            import sys, traceback
            print('Error in listener:', file=sys.stderr)
            traceback.print_exc(file=sys.stderr)


def worker_configurer(queue):
    h = logging.handlers.QueueHandler(queue)
    root = logging.getLogger()
    root.addHandler(h)
    root.setLevel(logging.DEBUG)


def worker_process(queue, configurer, name, callable):
    configurer(queue)
    logger = logging.getLogger(LOGGER)
    logger.info(f'Process {name} started...')
    callable(logger)
    logger.info(f'Process {name} ended.')


def solve_mps(logger, solver, mps, timeout):
    try:
        completed_process = subprocess.run(
            args=[solver, "--mps", mps, "--log", "info"], 
            timeout=timeout, 
            capture_output=True, 
            encoding="utf-8"
        )
        logger.info(completed_process.stdout)

        if completed_process.returncode == 0:
            logger.info("*** Success")
        else:
            logger.info("Non-zero return")

    except subprocess.TimeoutExpired:
        logger.info("Timeout")


def main():
    mps_directory = "mps"
    all_mps_files = Path(mps_directory).glob("*.mps")
    solver = Path("C:\\repos\\jsolve\\out\\build\\windows-release\\app\\jsolver_app.exe")
    max_solve_time = 600

    # Logging setup
    queue = mp.Queue(-1)
    listener = mp.Process(target=listener_process, args=(queue, listener_configurer))
    listener.start()
    
    worker_configurer(queue)
    logger = logging.getLogger(LOGGER)

    logger.info("Starting benchmark...")
    # Solve MPS files
    for i, mps_file in enumerate(all_mps_files):
        work_callable = partial(solve_mps, solver=solver, mps=mps_file, timeout=max_solve_time)
        p = mp.Process(target=worker_process, args=(queue, worker_configurer, mps_file.stem, work_callable))
        p.start()
        p.join()

    queue.put_nowait(None)
    listener.join() 


if __name__ == '__main__':
    main()
