def main():
	with open("/goinfre/psuanpro/42bangkok-webserv/aweb/cgi_python/index.html", "r") as f:
		content = f.read()
		print(content)


if __name__ == "__main__":
	main()
