import os


class Response:
	def __init__(self, content):
		self._res = f"HTTP/1.1 200 OK\r\nContent-Length: {len(content)}\r\n\r\n{content}"

	def __str__(self):
		print(self._res)

def ft_content():
	number1 = os.environ.get('number1')
	number2 = os.environ.get('number2')
	if number1 == None:
		number1 = 0
	if number2 == None:
		number2 = 0
	ret = ''
	ret+='<!DOCTYPE html>\n'
	ret+='<html>\n'
	ret+='<head>\n'
	ret+='\t<style>\n\t.link-button {background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px;}\n\t</style>'
	ret+='</head>\n'
	ret+='\t<body>\n'
	ret+=f'\t\t<h1>{number1} x {number2} = {int(number1) * int(number2)} </h1>\n'
	ret+='\t\t<a href="/index.html" class="link-button">GET</a>\n'
	ret+='\t</body>\n'
	ret+='</html>\n'
	return ret

def main():
	content = ft_content()
	res = Response(content)
	res.__str__()


if __name__ == "__main__":
	main()
