require 'readHTK'
require 'torch'

iptfile  = "440c020r.fbank"
optfile = 
Feature = readHTK.readFeature(file)
nSample = Feature['nSample']
sampSize = Feature['sampSize']
t = torch.Tensor(nSample, sampSize)
for i = 0, nSample-1 do
	for j = 0, sampSize-1 do
		t[i+1][j+1] = Feature[i * sampSize + j]
	end
end
