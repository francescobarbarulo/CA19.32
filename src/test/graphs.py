import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import sys
import getopt

def parse_if_number(s):
	try: return int(s)
	except: return True if s=="true" else False if s=="false" else s if s else None

def read_command_line(argv):
	print ('Number of arguments:',len(sys.argv),'arguments.')
	print ('Argument List:' + str(sys.argv))
	ini_file = ''
	nameStat = ''

	try:
		opts, args = getopt.getopt(argv,"hi:",["ifile="])
	except getopt.GetoptError:
		print ('test.py -i <inputfile>')
		sys.exit(2)
	for opt, arg in opts:
		if opt == '-h':
			print ('test.py -i <inputfile>')
			sys.exit()
		elif opt in ("-i", "--ifile"):
			ini_file = arg
		'''elif opt in ("-s", "--statname"):
			nameStat = arg'''
	return (ini_file)

def errorCreate(errorArray,std):
	for x in std:
		errorArray.append((x/(10)**(1/2))*2.262)

def main(argv):

	inputfile = read_command_line(argv)
	singleLineError = []
	mean =[]
	serie = []
	errorSerie = []
	ra_prob = np.arange(0,110,20)
	rr_prob = np.arange(20,90,20)


	print('Inputfile is: '+inputfile)
	names = ['sim', 'accessType', 'modeType', 'rand_access_prob', 'read_req_prob', 'prng_seed', 'ticks']
	try:
		csv = pd.read_csv(inputfile, delimiter=' ', names = names, converters =
								{'rand_access_prob':parse_if_number,
								 'read_req_prob':parse_if_number,
								 'prng_seed':parse_if_number,
								 'ticks':parse_if_number})
	except:
		print(name+': Il file non esiste')
	else:
		print(csv)
		'''dataSet = csv[(csv.read_req_prob == 60)]
		type = ['D','F']
		for i in type:
			for j in ra_prob:
				data = dataSet[(dataSet.rand_access_prob == j) & (dataSet.modeType == i)]['ticks']
				mean.append(data.mean())
				std = data.std()
				singleLineError.append((std/((10)**(1/2)))*3.250)   #Confidence Interval al 99%
			print("[modeType]"+ " [Mean array]\n")
			print(i + " "+ str(mean))
			serie.append(mean)
			errorSerie.append(singleLineError)
			mean = []
			singleLineError =[]

	colors = ['green', 'orange']
	labels=['Access Mode = Non-Optimized','Access Mode = Fast']

	fig, ax = plt.subplots()
	for i in range(0,len(serie)):
		plt.errorbar(ra_prob, serie[i], yerr = errorSerie[i], marker='D', capsize = 4, ms=3, mec='r',mfc='r', linestyle='-', linewidth=1,
		color=colors[i], ecolor='black')
	ax.set(title = 'Simulation with Fixed Read Request Probability (60%)', xlabel='Random Access Probability (%)', ylabel='Mean Number of Ticks')
	plt.xticks(ra_prob)
	ax.xaxis.set_major_locator(ticker.FixedLocator(ra_prob))
	#ax.xaxis.set_minor_locator(ticker.MaxNLocator(100))
	#ax.xaxis.set_ticklabels(diskSpeed,rotation=45)
	#plt.tick_params(x, labelsize=4)
	#plt.yticks(np.arange(0.,42,5))
	#plt.yscale('log')
	plt.legend(labels,loc=2, fontsize='small')
	ax.xaxis.grid(True, 'both', linewidth=0.2, alpha=0.8)
	ax.yaxis.grid(True, 'both', linewidth=0.2, alpha=0.8)
	plt.grid(True)
	plt.show()	'''
		serieD = []
		errorSerieD = []
		serieF = []
		errorSerieF = []

		dataSet = csv[(csv.modeType == 'D')]
		for i in rr_prob:
			for j in ra_prob:
				data = dataSet[(dataSet.rand_access_prob == j)&(dataSet.read_req_prob == i)]['ticks']
				mean.append(data.mean())
				print("[read_req_prob]"+ " [Mean array]\n")
				print(str(j ) + " "+ str(mean))
				std = data.std()
				singleLineError.append((std/((10)**(1/2)))*3.250)   #Confidence Interval al 99%
			serieD.append(mean)
			errorSerieD.append(singleLineError)
			mean = []
			singleLineError =[]

		dataSet = csv[(csv.modeType == 'F')]
		for i in rr_prob:
			for j in ra_prob:
				data = dataSet[(dataSet.rand_access_prob == j)&(dataSet.read_req_prob == i)]['ticks']
				mean.append(data.mean())
				print("[read_req_prob]"+ " [Mean array]\n")
				print(str(j ) + " "+ str(mean))
				std = data.std()
				singleLineError.append((std/((10)**(1/2)))*3.250)   #Confidence Interval al 99%
			serieF.append(mean)
			errorSerieF.append(singleLineError)
			mean = []
			singleLineError =[]

	colors = ['purple','g','orange','blue', 'grey', 'yellow']

	fig, ax = plt.subplots()
	for i in range(0,len(serieD)):
		plt.errorbar(ra_prob, serieD[i], yerr = errorSerieD[i], marker='D', capsize = 4, ms=3, mec='r',mfc='r', linestyle='--', linewidth=1,
		color=colors[i], ecolor='black',label='READ_REQ_PROB = '+str(rr_prob[i])+'%')
	for i in range(0,len(serieF)):
		plt.errorbar(ra_prob, serieF[i], yerr = errorSerieF[i], marker='D', capsize = 4, ms=3, mec='r',mfc='r', linestyle='-', linewidth=1,
		color=colors[i], ecolor='black')
	ax.set(title = 'Simple vs Fast Access Mode', xlabel='Random Access Probability (%)', ylabel='Mean Number of Ticks')
	plt.xticks(ra_prob)
	ax.xaxis.set_major_locator(ticker.FixedLocator(ra_prob))
	plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))

	#ax.xaxis.set_minor_locator(ticker.MaxNLocator(100))
	#ax.xaxis.set_ticklabels(diskSpeed,rotation=45)
	#plt.tick_params(x, labelsize=4)
	#plt.yticks(np.arange(0.,42,5))
	#plt.yscale('log')
	plt.legend(loc=2, fontsize='small')
	ax.xaxis.grid(True, 'both', linewidth=0.2, alpha=0.8)
	ax.yaxis.grid(True, 'both', linewidth=0.2, alpha=0.8)
	plt.grid(True)
	plt.show()

#	plt.savefig("ca.png");



if __name__ == '__main__':
	main(sys.argv[1:])
