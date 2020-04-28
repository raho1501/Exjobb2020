import numpy as np
import matplotlib.pyplot as plt
import csv
import pandas as pd

dataframe = pd.read_csv("ping_times.txt")

print(dataframe.head())

Q1 = dataframe['value'].quantile(0.25)
Q3 = dataframe['value'].quantile(0.75)
inter_quantile_range = Q3 - Q1

outliers = (dataframe['value'] < (Q1 - 1.5 * inter_quantile_range)) | (dataframe['value'] > (Q3 + 1.5 * inter_quantile_range))
percent_out = (outliers.sum()/dataframe['value'].count())*100
print("Percent outliers : ", percent_out)
print("Mean latency : ", np.mean(dataframe["value"]))
print("std latency : ", np.std(dataframe["value"]))
fig, axes = plt.subplots(1,2)
#box = dataframe.boxplot()
filtered_data = dataframe["value"].copy()
filtered_data[outliers] = np.nan
dataframe["filtered"] = filtered_data
#print(dataframe["value"])
#print(filtered_data)
dataframe.rename(columns={'value':'ping'}, inplace=True)
a = dataframe.boxplot("ping", ax=axes.flatten()[0])
a.set_ylabel('RTT latency (ms)')
a.set_title("RTT time for all tests")
text= "mean:" + str(np.round(np.mean(dataframe["ping"]),2)) + '\nstd:' + str(np.round(np.std(dataframe["ping"]),2))+"\n[ms]"
a.text(1.28,270,text,size=10, ha="center", bbox=dict(fc="none"))

a = dataframe.boxplot("filtered", ax=axes.flatten()[1])
a.set_ylabel('RTT latency (ms)')
a.set_title("Filtered RTT times with \noutliers("+str(np.round(percent_out,2))+"%) removed")
text= "mean:" + str(np.round(np.mean(dataframe["filtered"]),2)) + '\nstd:' + str(np.round(np.std(dataframe["filtered"]),2))+"\n[ms]"
a.text(1.3,8.3,text,size=10,ha="center", bbox=dict(fc="none"))

print("Mean filtered latency : ", np.mean(dataframe["filtered"]))
print("std filtered latency : ", np.std(dataframe["filtered"]))
plt.show()
