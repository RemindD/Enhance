require 'io'
require 'string'
require 'readHTK'
require 'torch'

function convertFile(fileName)
	file = io.open(fileName, "r")
	for Line in file:lines() do

		splt = stringSplit(Line, " ")
		inpt = splt[1]
		otpt = splt[2]

		Feat = readHTK.readFeature(inpt)

		nSample = Feat['nSample']
		sampSize = Feat['sampSize']

		t = torch.Tensor(nSample, sampSize)
		for i = 0, nSample-1 do
			for j = 0, sampSize-1 do
				t[i+1][j+1] = Feat[i * sampSize + j]
			end
		end
		torch.save(otpt, t)
	end
end

function stringSplit(string, onechar)
	res = {}
	stringLen = string.len(string)
	
	char = string.byte(onechar,1)
	stringIndex = 1
	Start = 1
	i = 1
	while (i<=stringLen) do
		if string.byte(string, i) == char then
			if i == Start then
				Start = Start + 1
				i = i + 1
			else
				res[stringIndex] = string.sub(string, Start, i-1)
				Start = i + 1
				stringIndex = stringIndex + 1
				i = i + 1
			end
		else
			i = i + 1
		end
	end
	if Start ~= i then
		res[stringIndex] = string.sub(string, Start, i-1)
	end
	return res
end

convertFile("scp")
