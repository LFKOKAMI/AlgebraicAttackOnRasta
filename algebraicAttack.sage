
n = 21
Round = 2

#EQB = []
#EQA = []
#EQL = []


U = 0
for i in range(2^(Round-1)+1 +1):
    U = U+binomial(n,i)

print('Number of monomials',U)

V = BooleanPolynomialRing(n,['k%d'%(i) for i in range(n)] )
V.inject_variables()

KEY_VARIABLES  =  list(V.gens())


KEY = []

for i in range(n):
    KEY.append(ZZ.random_element(2))

if(Round == 2):
    D_0 = U/(3*n)

if(Round == 3):
    D_0 = U/(5*n)

D_0 = ceil(D_0)

count = []
for i in range(101):
    count.append(0)


for number in range(100):
    POLYNOMIAL_SET = []
    MONOMIAL_SET = []
    for u in range(D_0+1): #D_0
         MATRIX_0 = Matrix(V,n)
         while(1):
              for i in range(n):
                   for j in range(n):
                        MATRIX_0[i,j] = ZZ.random_element(2)
              if(MATRIX_0.det() == 1):
                   break


         MATRIX_1 = Matrix(V,n)
         while(1):
              for i in range(n):
                   for j in range(n):
                        MATRIX_1[i,j] = ZZ.random_element(2)
              if(MATRIX_1.det() == 1):
                   break
         

         MATRIX_2 = Matrix(V,n)
         while(1):
              for i in range(n):
                   for j in range(n):
                        MATRIX_2[i,j] = ZZ.random_element(2)
              if(MATRIX_2.det() == 1):
                   break
         
          
         MATRIX_3 = Matrix(V,n)
         while(1):
              for i in range(n):
                   for j in range(n):
                        MATRIX_3[i,j] = ZZ.random_element(2)
              if(MATRIX_3.det() == 1):
                   break


         
         
         ROUND_CONSTANT_0 = []
         ROUND_CONSTANT_1 = []
         ROUND_CONSTANT_2 = []
         ROUND_CONSTANT_3 = []
         for _ in range(n):
             ROUND_CONSTANT_0.append(ZZ.random_element(2))
             ROUND_CONSTANT_1.append(ZZ.random_element(2))
             ROUND_CONSTANT_2.append(ZZ.random_element(2))
             ROUND_CONSTANT_3.append(ZZ.random_element(2))
             
     



         #Forward Direction
         LAST_FORWARD_ALPHA = [0]*n
         BETA = []
         for i in range(n):
              BETA.append(KEY_VARIABLES[i])
              
         for Round_Number in range(Round+1 ):
              if(Round_Number == 0):
                   ALPHA = MATRIX_0*vector(BETA)
                   for i in range(n):
                        ALPHA[i] = ALPHA[i]+ROUND_CONSTANT_0[i]  

              if(Round_Number == 1):
                   ALPHA = MATRIX_1*vector(BETA)
                   for i in range(n):
                        ALPHA[i] = ALPHA[i]+ROUND_CONSTANT_1[i]   


              if(Round_Number == 2):
                   ALPHA = MATRIX_2*vector(BETA)
                   for i in range(n):
                        ALPHA[i] = ALPHA[i]+ROUND_CONSTANT_2[i] 


              if(Round_Number == 3):
                   ALPHA = MATRIX_3*vector(BETA)
                   for i in range(n):
                        ALPHA[i] = ALPHA[i]+ROUND_CONSTANT_3[i] 


              

              if(Round_Number == Round):
                    continue

              for i in range(n):
                LAST_FORWARD_ALPHA[i] = ALPHA[i]
              
              for i in range(n):
                    gi = ALPHA[i]+ALPHA[(i+1)%n]*ALPHA[(i+2)%n]+ALPHA[(i+2)%n]
                    BETA[i] = gi





         
         
         #Forward Direction
         BETA_VALUE = []
         
         for i in range(n):
              BETA_VALUE.append(KEY[i]) #KEY[i] 
            

         for Round_Number in range(Round+1):
              if(Round_Number == 0):
                   ALPHA_VALUE = MATRIX_0*vector(BETA_VALUE)
                   for i in range(n):
                        ALPHA_VALUE[i] = ALPHA_VALUE[i]+ROUND_CONSTANT_0[i]  

              if(Round_Number == 1):
                   ALPHA_VALUE = MATRIX_1*vector(BETA_VALUE)
                   for i in range(n):
                        ALPHA_VALUE[i] = ALPHA_VALUE[i]+ROUND_CONSTANT_1[i]   


              if(Round_Number == 2):
                   ALPHA_VALUE = MATRIX_2*vector(BETA_VALUE)
                   for i in range(n):
                        ALPHA_VALUE[i] = ALPHA_VALUE[i]+ROUND_CONSTANT_2[i] 


              if(Round_Number == 3):
                   ALPHA_VALUE = MATRIX_3*vector(BETA_VALUE)
                   for i in range(n):
                        ALPHA_VALUE[i] = ALPHA_VALUE[i]+ROUND_CONSTANT_3[i] 


              
              if(Round_Number == Round):
                    continue

              for i in range(n):
                    gi = ALPHA_VALUE[i]+ALPHA_VALUE[(i+1)%n]*ALPHA_VALUE[(i+2)%n]+ALPHA_VALUE[(i+2)%n]
                    BETA_VALUE[i] = gi
               
         Z = []
         for i in range(n):
              Z.append(ALPHA_VALUE[i]+KEY[i])

          
         
         for i in range(n):
              Z[i] = Z[i]+KEY_VARIABLES[i]

         if(Round == 3):
              for i in range(n):
                 Z[i] = Z[i]+ROUND_CONSTANT_3[i] 

         if(Round == 2):
              for i in range(n):
                 Z[i] = Z[i]+ROUND_CONSTANT_2[i] 


         
         
         if(Round == 3): 
              Y = MATRIX_3^(-1)*vector(Z)      


         if(Round == 2): 
              Y = MATRIX_2^(-1)*vector(Z)      

        
         
         
         X = [0]*n

         for i in range(n):
              X[i] = LAST_FORWARD_ALPHA[i]

        
         

         counter = 0
         for i in range(n):
              if(len(POLYNOMIAL_SET) == U-1):
                   counter = 1
                   break
              f = Y[i]*(Y[(i-1)%n]+X[(i-1)%n])
              f = V(f)
              
              POLYNOMIAL_SET.append(f) 
              MONOMIAL_SET = union(MONOMIAL_SET,f.monomials()) 


         for i in range(n):
              if(len(POLYNOMIAL_SET) == U-1):
                   counter = 1
                   break
              f = Y[i]+X[i]+(Y[(i+1)%n]+1)*X[(i+2)%n]
              f = V(f)
              
              POLYNOMIAL_SET.append(f) 
              MONOMIAL_SET = union(MONOMIAL_SET,f.monomials()) 


         for i in range(n):
              if(len(POLYNOMIAL_SET) == U-1):
                   counter = 1
                   break
              f = Y[(i+3)%n]*(Y[(i+2)%n]*Y[(i+1)%n]+Y[(i+2)%n]+Y[i]+X[i])
              f = V(f)
              
              POLYNOMIAL_SET.append(f) 
              MONOMIAL_SET = union(MONOMIAL_SET,f.monomials()) 

         if(Round == 2):
            continue


         for i in range(n):
              if(len(POLYNOMIAL_SET) == U-1):
                   counter = 1
                   break
              f = (X[i]+X[(i+2)%n])*Y[(i+5)%n] + Y[i]*Y[(i+5)%n] + Y[(i+1)%n]*Y[(i+2)%n]*Y[(i+5)%n] + Y[(i+1)%n]*Y[(i+3)%n]*Y[(i+4)%n]*Y[(i+5)%n] + Y[(i+1)%n]*Y[(i+4)%n]*Y[(i+5)%n]
  
              f = V(f)
              
              POLYNOMIAL_SET.append(f) 
              MONOMIAL_SET = union(MONOMIAL_SET,f.monomials()) 


         for i in range(n):
              if(len(POLYNOMIAL_SET) == U-1):
                   counter = 1
                   break
              f = X[i]*Y[(7+i)%n] + Y[(3+i)%n]*Y[(4+i)%n]*Y[(7+i)%n]*Y[(1+i)%n] + Y[(3+i)%n]*Y[(4+i)%n]*Y[(7+i)%n] + Y[(3+i)%n]*Y[(5+i)%n]*Y[(6+i)%n]*Y[(7+i)%n]*Y[(1+i)%n] + Y[(3+i)%n]*Y[(5+i)%n]*Y[(6+i)%n]*Y[(7+i)%n] +       Y[(3+i)%n]*Y[(6+i)%n]*Y[(7+i)%n]*Y[(1+i)%n] + Y[(3+i)%n]*Y[(6+i)%n]*Y[(7+i)%n] + Y[(4+i)%n]*Y[(7+i)%n]*Y[(1+i)%n] + Y[(4+i)%n]*Y[(7+i)%n] + Y[(5+i)%n]*Y[(6+i)%n]*Y[(7+i)%n]*Y[(1+i)%n] + Y[(5+i)%n]*Y[(6+i)%n]*Y[(7+i)%n] +Y[(6+i)%n]*Y[(7+i)%n]*Y[(1+i)%n] + Y[(6+i)%n]*Y[(7+i)%n] + Y[(7+i)%n]*Y[i] + Y[(7+i)%n]*Y[(1+i)%n]*Y[(2+i)%n] + Y[(7+i)%n]*Y[(2+i)%n]
              f = V(f)
              
              POLYNOMIAL_SET.append(f) 
              MONOMIAL_SET = union(MONOMIAL_SET,f.monomials()) 




    print('Number of monomials',len(MONOMIAL_SET)) 
         
    print('Number of polynomials',len(POLYNOMIAL_SET))  


    #print(MONOMIAL_SET)
    

    



    

    MONOMIAL_SET_TEMP = []
   
    if((V(1) in MONOMIAL_SET) == True):
         MONOMIAL_SET_TEMP = [1]


    for i in range(len(KEY_VARIABLES)):
         if((KEY_VARIABLES[i] in MONOMIAL_SET) == True):
              MONOMIAL_SET_TEMP.append(KEY_VARIABLES[i])

    
    for i in range(len(MONOMIAL_SET)):
    	 if((V(MONOMIAL_SET[i]) in MONOMIAL_SET_TEMP) == False):
              MONOMIAL_SET_TEMP.append(MONOMIAL_SET[i])
              

    
    for i in range(len(MONOMIAL_SET)):
         MONOMIAL_SET[i] = MONOMIAL_SET_TEMP[i]

    if((V(1) in MONOMIAL_SET) == True):
         MONOMIAL_SET.remove(V(1))
    
    INVOLVED_KEY = 0

    for i in range(n):
         if((KEY_VARIABLES[i] in MONOMIAL_SET) == True):
              INVOLVED_KEY = INVOLVED_KEY+1
    COEFFICIENT_MATRIX = zero_matrix(GF(2), len(POLYNOMIAL_SET), len(MONOMIAL_SET))
    B = [V(0)]*len(POLYNOMIAL_SET)

    print(len(POLYNOMIAL_SET), len(MONOMIAL_SET))
    
    for i in range(len(POLYNOMIAL_SET)):
         f = POLYNOMIAL_SET[i]
         S = f.monomials()
         if((V(1) in S) == True):
              B[i] = V(1)
       
         for j in range(len(MONOMIAL_SET)):
              if((MONOMIAL_SET[j] in S) == True):
                   COEFFICIENT_MATRIX[i,j] = 1


    
        
    #EQA.append(COEFFICIENT_MATRIX.rank())
    #EQB.append(len(POLYNOMIAL_SET))
    #EQL.append(INVOLVED_KEY)
    #print(EQA,EQB,EQL)
    count[len(POLYNOMIAL_SET)-COEFFICIENT_MATRIX.rank()] = count[len(POLYNOMIAL_SET)-COEFFICIENT_MATRIX.rank()] + 1
    #if(COEFFICIENT_MATRIX.rank() == len(MONOMIAL_SET)):
         #X = (COEFFICIENT_MATRIX)^(-1)*vector(B)
         #for i in range(n):
              #print(X[i], end  =  ' ') 
         #print(" ") 
         #for i in range(n):
              #print(KEY[i], end  =  ' ') 
         #print(" ")
    print('current times:',number+1)
    print('EQB - EQA:',end = '\n')
    for i in range (101):
        if (count[i]!=0):
            print('%d : %d' % (i,count[i]))
    print('\n')

     
