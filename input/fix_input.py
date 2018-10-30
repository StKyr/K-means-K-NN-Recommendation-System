#!/usr/bin/python

from sys import argv

inp_name = argv[1]

cnt = 0

with open(inp_name, "r") as fin:

	with open(inp_name+".fixed", "w") as fout:

		line = fin.readline()
		while line:
			if (cnt == 0 and line=="euclidean") or (cnt==0 and line=="cosine") or (cnt==0 and "Radius" in line) or line is None or line == '':
				fout.write(line)
			else:
				newline = line.replace(' ', '\t')
				fout.write("id_"+str(cnt)+"\t"+newline)
			
			cnt += 1
			line = fin.readline()






