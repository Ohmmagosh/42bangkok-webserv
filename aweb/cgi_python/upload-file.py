#!/usr/bin/env python3

import sys
import cgitb

# Enable debugging
cgitb.enable()

# Read the content from stdin
input_data = sys.stdin.read()

# Save the input data to a file
with open("input.txt", "w") as file:
    file.write(input_data)

# Send response header
print("Content-Type: text/plain\n")

# Output the response body
print("finish!!!")