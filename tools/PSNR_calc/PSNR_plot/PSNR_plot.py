import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

for plot in range(1,12):
    data4G = pd.read_csv("results/4gp"+str(plot)+"_psnr_result.csv")
    mean4G = []
    std4G = []
    for column in data4G:
        mean4G.append(data4G[column].mean())
        std4G.append(data4G[column].std())

    data5G = pd.read_csv("results/5gp"+str(plot)+"_psnr_result.csv")
    mean5G = []
    std5G = []
    for column in data5G:
        mean5G.append(data5G[column].mean())
        std5G.append(data5G[column].std())
    x=data4G.keys()
    
    plt.figure(plot)
    plt.subplot(121)
    plt.plot(x,mean4G)
    plt.plot(x,mean5G)
    plt.xticks([0,29,59,89,119,149,179])
    plt.xlabel("Frame number")
    plt.ylabel("PSNR [dB]")
    plt.title("Mean PSNR plot for measuring point p"+str(plot))
    plt.legend(["4G","5G"])

    plt.subplot(122)
    plt.plot(x,std4G)
    plt.plot(x,std5G)
    plt.xticks([0,29,59,89,119,149,179])
    plt.xlabel("Frame number")
    plt.ylabel("PSNR [dB]")
    plt.title("Standard Deviation of PSNR plot for measuring point p"+str(plot))
    plt.legend(["4G","5G"])

    plt.show()

#reliabilitydata = []
#with open("results/video_errors.txt","r") as file:
#    reliabilitydata = list(file)

#for word in reliabilitydata:
#    word = word.strip()
#plot_data = []
#for j in range(0,660,30):
#    dataslice = []
#    temp_data = reliabilitydata[j:j+30]
#    index = temp_data[0].find(':')
#    title = temp_data[0][3:index-4]
#    dataslice.append(title)
#    for datapoint in temp_data:
#        dataslice.append(datapoint[index+1:-1])
#    plot_data.append(dataslice)

#plot_data= np.array(plot_data)
#print(plot_data)
#min_4g = []
#max_4g = []
#avg_4g = []
#min_5g = []
#max_5g = []
#avg_5g = []


#for i in range(0,11):
#    data=plot_data[i][1:30]
#    data=np.array(data,dtype=np.float64)
#    min_4g.append(data.min())
#    max_4g.append(data.max())
#    avg_4g.append(data.mean())
#for i in range(11,22):
#    data=plot_data[i][1:30]
#    data=np.array(data,dtype=np.float64)
#    min_5g.append(data.min())
#    max_5g.append(data.max())
#    avg_5g.append(data.mean())
#x= [0,1,2,3,4,5,6,7,8,9,10]
#xtics = ["p1","p2","p3","p4","p5","p6","p7","p8","p9","p10","p11"]
#plt.figure(12)
#plt.subplot(131)
#plt.plot(x,min_4g)
#plt.plot(x,min_5g)
#plt.xticks(np.arange(11),xtics)
#plt.xlabel("Measuring point")
#plt.ylabel("Fraction recieved")
#plt.title("Min received frames of 30 runs")
#plt.legend(["4G","5G"])

#plt.subplot(132)
#plt.plot(x,avg_4g)
#plt.plot(x,avg_5g)
#plt.xticks(np.arange(11),xtics)
#plt.xlabel("Measuring point")
#plt.ylabel("Fraction recieved")
#plt.title("Avg. received frames of 30 runs")
#plt.legend(["4G","5G"])

#plt.subplot(133)
#plt.plot(x,max_4g)
#plt.plot(x,max_5g)
#plt.xticks(np.arange(11),xtics)
#plt.xlabel("Measuring point")
#plt.ylabel("Fraction recieved")
#plt.title("Max received frames of 30 runs")
#plt.legend(["4G","5G"])

#plt.show()