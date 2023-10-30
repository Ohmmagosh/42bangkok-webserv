import os

def header():
	print('<head>')
	print('<title>Multiply</title>')
	tagElement('style', '.link-button {background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px;}')
	print('</head>')

def	body():
	number1 = os.environ.get('number1')
	number2 = os.environ.get('number2')
	if number1 == None:
		number1 = 0
	if number2 == None:
		number2 = 0
	print('<body>')
	print(f'<h1>{number1} x {number2} = {int(number1) * int(number2)} </h1>')
	button()
	print('</body>')

def tagElement(tag, text):
	print(f'<{tag}>{text}</{tag}>')

def button():
	print('<a href="/index.html" class="link-button">GET</a>')

def html():
	print('<!DOCTYPE html>')
	print('<html>')
	header()
	body()
	print('</html>')

def main():
	html()

if __name__ == "__main__":
	main()
