import random
import os

def MakeInput():
	with open("input/input.txt", "w") as output:
		for i in xrange(1000):
			output.write(random.choice(["I", "D", "S", "R"]))
			output.write(" ")
			output.write(str(random.randint(1, 999)))
			output.write(" ")
	with open("input/input1.txt", "w") as output:
		for i in xrange(10000):
			output.write(random.choice(["I", "D", "S", "R"]))
			output.write(" ")
			output.write(str(random.randint(1, 999)))
			output.write(" ")
	with open("input/input2.txt", "w") as output:
		for i in xrange(1000):
			output.write(random.choice(["I", "D", "S", "R"]))
			output.write(" ")
			output.write(str(random.randint(1, 99)))
			output.write(" ")
	with open("input/input3.txt", "w") as output:
		for i in xrange(10000):
			output.write(random.choice(["I", "D", "S", "R"]))
			output.write(" ")
			output.write(str(random.randint(1, 99)))
			output.write(" ")
	with open("input/input4.txt", "w") as output:
		for i in xrange(1000):
			output.write(random.choice(["I", "I", "I", "I", "I", "D", "D", "D", "D", "D", "S", "R"]))
			output.write(" ")
			output.write(str(random.randint(1, 999)))
			output.write(" ")
	with open("input/input5.txt", "w") as output:
		for i in xrange(10000):
			output.write(random.choice(["I", "I", "I", "I", "I", "D", "D", "D", "D", "D", "S", "R"]))
			output.write(" ")
			output.write(str(random.randint(1, 999)))
			output.write(" ")
	with open("input/input6.txt", "w") as output:
		for i in xrange(1000):
			output.write(random.choice(["I", "I", "I", "I", "I", "D", "D", "D", "D", "D", "S", "R"]))
			output.write(" ")
			output.write(str(random.randint(1, 99)))
			output.write(" ")
	with open("input/input7.txt", "w") as output:
		for i in xrange(10000):
			output.write(random.choice(["I", "I", "I", "I", "I", "D", "D", "D", "D", "D", "S", "R"]))
			output.write(" ")
			output.write(str(random.randint(1, 99)))
			output.write(" ")

def Init():
	os.system("rm ./input/*")
	os.chdir("./os")
	os.system("make clean")
	os.system("make all")
	os.chdir("..")
	os.chdir("./checker")
	os.system("make clean")
	os.system("make all")
	os.chdir("..")

def OSRun():
	os.chdir("./os")
	os.system('make run IN="../input/input.txt" OUT="../input/output.txt"')
	os.system('make run IN="../input/input1.txt" OUT="../input/output1.txt"')
	os.system('make run IN="../input/input2.txt" OUT="../input/output2.txt"')
	os.system('make run IN="../input/input3.txt" OUT="../input/output3.txt"')
	os.system('make run IN="../input/input4.txt" OUT="../input/output4.txt"')
	os.system('make run IN="../input/input5.txt" OUT="../input/output5.txt"')
	os.system('make run IN="../input/input6.txt" OUT="../input/output6.txt"')
	os.system('make run IN="../input/input7.txt" OUT="../input/output7.txt"')
	os.chdir("../")

def CheckerRun():
	os.chdir("./checker")
	os.system('make run IN="../input/input.txt" OUT="../input/output.txt"')
	os.system('make run IN="../input/input1.txt" OUT="../input/output1.txt"')
	os.system('make run IN="../input/input2.txt" OUT="../input/output2.txt"')
	os.system('make run IN="../input/input3.txt" OUT="../input/output3.txt"')
	os.system('make run IN="../input/input4.txt" OUT="../input/output4.txt"')
	os.system('make run IN="../input/input5.txt" OUT="../input/output5.txt"')
	os.system('make run IN="../input/input6.txt" OUT="../input/output6.txt"')
	os.system('make run IN="../input/input7.txt" OUT="../input/output7.txt"')
	os.chdir("../")

if __name__ == "__main__":
	Init()
	MakeInput()
	OSRun()
	CheckerRun()
