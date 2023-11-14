#!/usr/bin/env python3

import cgi
import os

class Response:
	def __init__(self, content, filename):
		self._res = f"HTTP/1.1 200 OK\r\nContent-Length: {len(content)}\r\nContent-Type: application/octet-stream\r\nContent-Disposition: attachment; filename={filename}\r\n\r\n{content}"

	def __str__(self):
		print(self._res)

def main():

    # Get filename from the query parameter
    filename = os.environ.get('filename')

    # Check if the file exists and serve it, else throw a 404
    file_path = os.path.join("/Users/rchiewli/webser/aweb/upload/", filename)
    # file_path = os.environ.get('save_path')

    if os.path.exists(file_path):
        # print("HTTP/1.1 200 OK")
        # print("Content-Type: application/octet-stream")
        # print(f"Content-Disposition: attachment; filename={filename}")
        # print()
        with open(file_path, 'r') as f:
            ret = f.read()
            # print(f.read())
        res = Response(ret, filename)
        res.__str__()

    else:
        print("Content-Type: text/plain")
        print("Status: 404 Not Found")
        print()
        print("Error 404: File not found")

if __name__ == '__main__':
    main()
