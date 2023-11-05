#!/usr/bin/env python3

# Import the required modules
import cgitb
import time

# Enable CGI traceback for debugging
cgitb.enable()

# Print HTTP header
print("Content-Type: text/plain;charset=utf-8")
print()

print("Starting infinite loop... (Please terminate this manually!)")

# Start infinite loop
while True:
    print("Looping...")
    time.sleep(1)  # Sleep for 1 second to prevent this loop from consuming too much CPU