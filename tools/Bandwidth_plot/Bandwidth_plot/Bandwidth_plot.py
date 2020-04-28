import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
file_name = "bitrate tests.xlsx"
xl_file = pd.ExcelFile(file_name)

dfs = {sheet_name: xl_file.parse(sheet_name) 
          for sheet_name in xl_file.sheet_names}

#print(dfs)

upload_sheet = dfs.get("Upload (Mbps)")
download_sheet = dfs.get("Download (Mbps)")

#data
mean_upload_4G = []
std_upload_4G = []
mean_download_4G = []
std_download_4G = []

mean_upload_5G = []
std_upload_5G = []
mean_download_5G = []
std_download_5G = []

for i in range(1,12):
    mean_upload_4G.append(upload_sheet["4gp"+str(i)].mean())
    std_upload_4G.append(upload_sheet["4gp"+str(i)].std())
    mean_download_4G.append(download_sheet["4gp"+str(i)].mean())
    std_download_4G.append(download_sheet["4gp"+str(i)].std())

    mean_upload_5G.append(upload_sheet["5gp"+str(i)].mean())
    std_upload_5G.append(upload_sheet["5gp"+str(i)].std())
    mean_download_5G.append(download_sheet["5gp"+str(i)].mean())
    std_download_5G.append(download_sheet["5gp"+str(i)].std())

mean_upload_4G = np.array(mean_upload_4G)
std_upload_4G = np.array(std_upload_4G)
mean_download_4G = np.array(mean_download_4G)
std_download_4G = np.array(std_download_4G)

mean_upload_5G = np.array(mean_upload_5G)
std_upload_5G = np.array(std_upload_5G)
mean_download_5G = np.array(mean_download_5G)
std_download_5G = np.array(std_download_5G)
labels = ["p1","p2","p3","p4","p5","p6","p7","p8","p9","p10","p11"]
x = np.arange(len(labels))  # the label locations
width = 0.35  # the width of the bars
fig, ax = plt.subplots()
ax = plt.subplot(121)
ax.bar(x - width/2,mean_upload_4G, width=width, yerr=std_upload_4G)
ax.bar(x + width/2,mean_upload_5G,width=width, yerr=std_upload_5G)
ax.set_ylabel('Upload Speed [Mbps]')
ax.set_xlabel('Measuring point')
ax.set_title('Upload speeds at various points around campus')
ax.set_xticks(x)
ax.set_xticklabels(labels)
ax.legend(["4G","5G"])

ax = plt.subplot(122)
ax.bar(x - width/2,mean_download_4G, width=width, yerr=std_download_4G)
ax.bar(x + width/2,mean_download_5G,width=width, yerr=std_download_5G)
ax.set_ylabel('Download Speed [Mbps]')
ax.set_xlabel('Measuring point')
ax.set_title('Download speeds at various points around campus')
ax.set_xticks(x)
ax.set_xticklabels(labels)
ax.legend(["4G","5G"])
plt.show()