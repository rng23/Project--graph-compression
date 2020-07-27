import matplotlib.pyplot as plt 

#caida
x1 = [3, 4, 5, 6, 7, 8, 9, 10]
y1 = [21635, 21359, 21301, 21099, 20979, 20829, 20835, 20651]
plt.plot(x1, y1, label = "caida")

#livemocha
x2 = [3, 4, 5, 6, 7, 8, 9, 10]
y2 = [50581, 49588, 49181, 47338, 46808, 45721, 43084, 40832]
plt.plot(x2, y2, label = "livemocha")

#amazon
x3 = [3, 4, 5, 6, 7, 8, 9, 10]
y3 = [161640, 160311, 157887, 154744, 152590, 149977, 148181, 146337]
plt.plot(x3, y3, label = "amazon")

#hyves
x4 = [3, 4, 5, 6, 7, 8, 9, 10]
y4 = [1201428, 1189625, 1173607, 1162354, 1152841, 1139143, 1123875, 1109071]
plt.plot(x4, y4, label = "hyves")

#com-dblp
x5 = [3, 4, 5, 6, 7, 8, 9, 10]
y5 = [126046, 125674, 124469, 121048, 119144, 116216, 114374, 113921]
plt.plot(x5, y5, label = "com-dblp")

#wordnet
x6 = [3, 4, 5, 6, 7, 8, 9, 10]
y6 = [67486, 66308, 66103, 65674, 64801, 63690, 62756, 62662]
plt.plot(x6, y6, label = "wordnet")

#protein
x7 = [3, 4, 5, 6, 7, 8, 9, 10]
y7 = [1285, 1253, 1236, 1239, 1218, 1206, 1211, 1215]
plt.plot(x7, y7, label = "protein")

#dolphins
x8 = [3, 4, 5]
y8 = [10, 10, 13]
plt.plot(x8, y8, label = "dolphins")

# naming the x axis 
plt.xlabel('Isolated vertices') 
# naming the y axis 
plt.ylabel('Compression rate') 
# giving a title to my graph 
plt.title('Isolated vertices  x Compression rate') 
  
# show a legend on the plot 
plt.legend() 
  
# function to show the plot 
plt.show() 