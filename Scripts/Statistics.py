file = 'D:/OpenGLProjects/Spices-Engine/premake5.lua'

with open(file, 'r') as file: 
	numLines = sun(1 for line in file)

print(numLines)