from PIL import Image
import copy

columns=30 
rows=15

color=[]
move=[]

orig = Image.open("untitled2.png")
rgb_orig = orig.convert('RGB')

blockx = orig.size[0]/columns
blocky = orig.size[1]/rows 

for i in range(rows):
    
    temp=[]
    
    for j in range(columns):
        r,g,b = rgb_orig.getpixel((((blockx/2)+(j*blockx)),((blocky/2)+(i*blocky))));
        if r == 0 and g == 0 and b == 0 :
            temp.append(1)
        elif r == 255 and g == 255 and b == 255 :
            temp.append(0)
        elif r == 255 and g == 0 and b == 0 :
            temp.append(2)
        elif r == 0 and g == 0 and b == 255 :
            temp.append(3)
        else :
            temp.append(4)
            
    color.append(temp)

move=copy.deepcopy(color)
  
for i in range(rows):
    for j in range(columns):
        temp = []
        if color[i][j] == 1:
            move[i][j]=[0,0,0,0]
        else:
            #top
            if i == 0:
                temp.append(0)
            else:
               if color[i-1][j]!=1:
                   temp.append(1)
               else: 
                   temp.append(0)
                    
            #right
            if j == columns-1:
                temp.append(0)
            else:
               if color[i][j+1]!=1:
                   temp.append(1)
               else: 
                   temp.append(0)       
        
            #bottom                            
            if i == rows-1:
                temp.append(0)
            else:
               if color[i+1][j]!=1:
                   temp.append(1)
               else: 
                   temp.append(0)
                   
            #left
            if j == 0:
                temp.append(0)
            else:
               if color[i][j-1]!=1:
                   temp.append(1)
               else: 
                   temp.append(0)
            
            move[i][j]=temp
                                          
print(color)

for i in move:
    for j in i:
        print j
        

file1 = open('contour_map.txt','w')
file2 = open('movement_map.txt','w')

for i in color:
    for j in i:
        file1.write(str(j))
        
file1.close()

for i in move:
    for j in i:
        for k in j:
            file2.write(str(k))

file2.close()