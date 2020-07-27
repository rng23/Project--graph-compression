import matplotlib.pyplot as plt 

#caida
x1 = [3, 4, 5, 6, 7, 8, 9, 10]
y1 = [50277, 49958, 49856, 49620, 49437, 49281, 49216, 49028]
plt.plot(x1, y1, label = "caida")

#livemocha
x2 = [3, 4, 5, 6, 7, 8, 9, 10]
y2 = [2158572, 2158708, 2156547, 2153974, 2152867, 2151204, 2147309, 2144146]
plt.plot(x2, y2, label = "livemocha")

#amazon
x3 = [3, 4, 5, 6, 7, 8, 9, 10]
y3 = [781389, 774407, 759851, 743069, 728993, 714642, 702117, 691949]
plt.plot(x3, y3, label = "amazon")

#hyves
x4 = [3, 4, 5, 6, 7, 8, 9, 10]
y4 = [2641293, 2628129, 2612191, 2599144, 2588191, 2573558, 2556944, 2541236]
plt.plot(x4, y4, label = "hyves")

#com-dblp
x5 = [3, 4, 5, 6, 7, 8, 9, 10]
y5 = [884503, 875713, 860008, 841424, 825842, 809723, 797894, 789530]
plt.plot(x5, y5, label = "com-dblp")

#wordnet
x6 = [3, 4, 5, 6, 7, 8, 9, 10]
y6 = [590871, 586892, 581537, 575813, 569996, 564653, 558408, 555707]
plt.plot(x6, y6, label = "wordnet")

#protein
x7 = [3, 4, 5, 6, 7, 8, 9, 10]
y7 = [1839, 1800, 1773, 1765, 1740, 1727, 1734, 1728]
plt.plot(x7, y7, label = "protein")

#dolphins
x8 = [3, 4, 5]
y8 = [122, 122, 123]
plt.plot(x8, y8, label = "dolphins")

# naming the x axis 
plt.xlabel('External edges') 
# naming the y axis 
plt.ylabel('Compression rate') 
# giving a title to my graph 
plt.title('External edges  x Compression rate') 
  
# show a legend on the plot 
plt.legend() 
  
# function to show the plot 
plt.show() 