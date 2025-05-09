#!/bin/bash

set -e
source utils/colors.sh

print_help() {
  echo "Usage: ./build.sh [options]"
  echo ""
  echo "Options:"
  echo "  --disk        Build the disk_usage module"
  echo "  --date        Build the date module"
  echo "  --help        Show this help message"
  echo ""
  echo "If no options are provided, all modules are built."
  exit 0
}

build_disk() {
  echo -e "$BLUE Building disk_usage...$RESET"
  clang -O2 -s disk_usage/disk_usage.c -o disk_usage/disk_usage
  echo "$GREEN Built disk_usage.$RESET"
}

build_date() {
  echo "$BLUE Building date...$RESET"
  clang -O2 -s date/date.c -o date/date
  echo "$GREEN Built date.$RESET"
}

# No arguments: build all
if [ $# -eq 0 ]; then
  build_disk
  build_date
  exit 0
fi

# Parse arguments
while [[ "$#" -gt 0 ]]; do
  case "$1" in
    --disk) build_disk ;;
    --date) build_date ;;
    --help) print_help ;;
    *) echo "󰯆 Unknown option: $1"; print_help ;;
  esac
  shift
done
