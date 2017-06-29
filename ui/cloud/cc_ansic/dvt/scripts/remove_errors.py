"""A simple main to fix the config.c file used for samples"""
import sys
sys.path.insert(0, "dvt/cases")
from build_utils import setup_platform
from config import update_config_header

if __name__ == "__main__":
    setup_platform(*sys.argv[1:])
