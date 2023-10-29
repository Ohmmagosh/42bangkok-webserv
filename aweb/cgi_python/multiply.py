import os

def header():
	print('<head>')
	print('<title>Multiply</title>')
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
	print('</body>')

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
