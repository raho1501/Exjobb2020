import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def file_to_df(file):
    data = []
    for line in file:
        data.append(list(line.split(',')))

    data = np.array(data)
    dataframe = pd.DataFrame()
    for row in data:
        label = row[0][2:]
        values =  np.array(row[1:31],dtype=np.int64)
        dataframe[label] = values

    return dataframe.copy()

def main():
    file_4G = open("latency_results_4G_ns.txt", "r")
    file_5G = open("latency_results_5G_ns.txt", "r")
    data_4G = file_to_df(file_4G)
    data_5G = file_to_df(file_5G)
    file_4G.close()
    file_5G.close()

    data_4G = data_4G.div(10**6) #convert from ns to ms
    data_5G = data_5G.div(10**6)

    print(data_4G.head())
    print(data_5G.head())
    print(data_4G.std())
    print(data_5G.std())
    pd.concat([data_4G.mean().rename('4G mean RTT'), data_5G.mean().rename('5G mean RTT')],axis=1).plot.bar(yerr=[data_4G.std(),data_5G.std()])
    plt.xlabel('Measuring point')
    plt.ylabel('Latency [ms]')
    plt.title("Latency comparison 4G/5G")
    plt.show()

if __name__ == '__main__':
    main()