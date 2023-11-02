#!/usr/bin/env python3

import sys
import cgitb
import os
import re

# Enable debugging
cgitb.enable()

# for key, value in os.environ.items():
#     sys.stderr.write(f"{key}={value}\n")
save_path = os.environ.get('save_path')

header = os.environ.get('Content-Disposition', '')
match = re.search(r'filename="([^"]+)"', header)
if match:
    filename = match.group(1)
    full_save_path = os.path.join(save_path, filename)
    sys.stderr.write(full_save_path)
else:
    sys.stderr.write("Filename not found")



# sys.stderr.write(save_path)

# Read the content from stdin
input_data = sys.stdin.read()

# Save the input data to a file
with open(full_save_path, "w") as file:
    file.write(input_data)


# Output the response body
print("<h1>HELLO</h1>")