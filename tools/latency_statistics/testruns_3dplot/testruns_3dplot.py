import numpy as np
import matplotlib.pyplot as plt
import csv
import pandas as pd


file_name = "LatencyBookForTests.xlsx"
xl_file = pd.ExcelFile(file_name)

dfs = {sheet_name: xl_file.parse(sheet_name) 
          for sheet_name in xl_file.sheet_names}

latency_column = []
hits_column = []
distance1_column = []
distance2_column = []
time_column = []
userscore_column = []
precision_wall1_hits = []
precision_wall2_hits = []

hits_std_column = []
distance1_std_column = []
distance2_std_column = []
time_std_column = []
userscore_std_column = []
for sheet in dfs:
    #print(sheet)
    data = dfs.get(sheet)
    #print(type(data))
    latency_column.append(int(sheet[0:-2]))
    hits_column.append(np.mean(data["hits"]))
    hits_std_column.append(np.std(data["hits"]))

        
    dist1 = [float(x) for x in data["dist1"] if x != "krock" ]
    distance1_column.append( np.mean(dist1) )
    distance1_std_column.append( np.std(dist1) )
    precision_wall1_hits.append(len(data["dist1"]) - len(dist1))
    dist2 = [ float(x) for x in data["dist2"] if x != "krock" ]
    distance2_column.append( np.mean(dist2) )
    distance2_std_column.append( np.std(dist2) )
    precision_wall2_hits.append(len(data["dist2"]) - len(dist2))

    time_column.append( np.mean(data["time"]) )
    time_std_column.append( np.std(data["time"]) )

    userscore_column.append( np.mean(data["userscore(1-8)"]) )
    userscore_std_column.append( np.std(data["userscore(1-8)"]) )
print("sheet done \n\n\n")
X, Y, Z = np.array(latency_column), np.array(hits_column), np.array(time_column)
Ys, Zs = np.array(hits_std_column), np.array(time_std_column)
fig = plt.figure()
ax = fig.add_subplot(231, projection='3d')
ax.set_title("3D-plot of latency introduced, collisions and test-time for users")
surf=ax.plot_trisurf(X, Y, Z, color="black", label="avg")
surf._facecolors2d=surf._facecolors3d
surf._edgecolors2d=surf._edgecolors3d
surf=ax.plot_trisurf(X, Y+Ys, Z+Zs, color="red",label="avg +"+ r'$\sigma$' )
surf._facecolors2d=surf._facecolors3d
surf._edgecolors2d=surf._edgecolors3d
surf=ax.plot_trisurf(X, Y-Ys, Z-Zs, color="blue",label="avg -"+r'$\sigma$')
surf._facecolors2d=surf._facecolors3d
surf._edgecolors2d=surf._edgecolors3d
ax.set_xticks(X)
ax.set_xticklabels(X,rotation=5)
ax.legend(loc='upper right', bbox_to_anchor=(0.2, 0.90), shadow=False)
ax.set(xlabel="Latency introduced[ms]", ylabel="Collisions[ct.]",zlabel="Time[s]")

ax = fig.add_subplot(232)
ax.plot(X, Y, color="black", label="avg")
ax.plot(X, Y+Ys, color="red", label="avg +"+ r'$\sigma$')
ax.plot(X, Y-Ys, color="blue", label="avg -"+r'$\sigma$')
ax.set_xticks(X)
ax.set(xlabel="Latency introduced[ms]", ylabel="Collisions[ct.]")
ax.set_title("Latency and collisions plot")
ax.legend()

ax = fig.add_subplot(233)
ax.plot(X, Z, color="black", label="avg")
ax.plot(X, Z+Zs, color="red", label="avg +"+ r'$\sigma$')
ax.plot(X, Z-Zs, color="blue", label="avg -"+r'$\sigma$')
ax.set(xlabel="Latency introduced[ms]", ylabel="Time[s]")
ax.set_xticks(X)
ax.set_title("Latency introduced and time plot")
ax.legend()

ax = fig.add_subplot(234)
Y, Ys= np.array(distance1_column), np.array(distance1_std_column)
ax.plot(X, Y, color="black", label="avg")
for i in range(len(precision_wall1_hits)):
    ax.text(X[i],26,str(precision_wall1_hits[i])+'x')
ax.plot(X, Y+Ys, color="red", label="avg +"+ r'$\sigma$')
ax.plot(X, Y-Ys, color="blue", label="avg -"+r'$\sigma$')
ax.set_xticks(X)
ax.set(xlabel="Latency introduced[ms]", ylabel="Distance from wall[cm]")
ax.set_title("Latency introduced and distance from wall 1 plot, with number of wall crashes")
ax.legend(loc='upper right', bbox_to_anchor=(0.2, 0.90), shadow=False)

ax = fig.add_subplot(235)
Y, Ys= np.array(distance2_column), np.array(distance2_std_column)
for i in range(len(precision_wall2_hits)):
    ax.text(X[i],25.5,str(precision_wall2_hits[i])+'x')
ax.plot(X, Y, color="black", label="avg")
ax.plot(X, Y+Ys, color="red", label="avg +"+ r'$\sigma$')
ax.plot(X, Y-Ys, color="blue", label="avg -"+r'$\sigma$')
ax.set_xticks(X)
ax.set(xlabel="Latency introduced[ms]", ylabel="Distance from wall[cm]")
ax.set_title("Latency introduced and distance from wall 2 plot, with number of wall crashes")
ax.legend(loc='upper right', bbox_to_anchor=(0.2, 0.90), shadow=False)

ax = fig.add_subplot(236)
Y, Ys= np.array(userscore_column), np.array(userscore_std_column)
ax.plot(X, Y, color="black", label="avg")
ax.plot(X, Y+Ys, color="red", label="avg +"+ r'$\sigma$')
ax.plot(X, Y-Ys, color="blue", label="avg -"+r'$\sigma$')
ax.set_xticks(X)
ax.set(xlabel="Latency introduced[ms]", ylabel="Userscore[pt]")
ax.set_title("Latency introduced and userscore plot")
ax.legend()

plt.show()