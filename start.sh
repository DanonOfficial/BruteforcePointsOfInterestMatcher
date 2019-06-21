#!/usr/bin/env bash

./bin/EntryTask "$1" "$2"
echo "$1"
echo "$2"
python graphPlotter.py