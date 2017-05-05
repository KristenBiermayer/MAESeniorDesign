clear all 
close all 
clc 



%Setting fixed parameters (units:mm)
r2 = 12.7*(1/1000) ;
%r3 = 111.7*(1/1000);
r3=112*(1/1000)
r4 = 15*(1/1000);
r1x = 107.95*(1/1000);
r1y=18.796*(1/1000);

%setting up storage
rr = 100;
theta2_maximum = pi;
theta2_minimum = 0;
theta2 = linspace(theta2_minimum ,theta2_maximum,rr);
t3 = zeros(1,rr);
t4 = zeros(1,rr);

%initial guesses 

t3_1 = 200*pi/180;
t4_1 = 250*pi/180;



%setting for loop
for jj = 1:rr;
    
%Set dx and dy to a randomly large number
dx1 = 100;
dx2 = 100;
D = [dx1; dx2];

%Set up variables for iterations
n = 0;

while norm(D,1) > 10^(-6);
    n = n+1;
    %Evaluate f1, f2
    f1=r2*cos(theta2(jj))+r3*cos(t3_1)+r4*cos(t4_1)+r1x
    f2 = r2*sin(theta2(jj)) + r3*sin(t3_1) + r4*sin(t4_1)+r1y;
    
    
    F = [f1;
        f2;];
  
   %Evaluate Jacobian
   J11 = -r3*sin(t3_1);
   J12 = -r4*sin(t4_1);
   J21 = r3*cos(t3_1);
   J22 = r4*cos(t4_1);
  
   J = [J11 J12;
       J21 J22;];
   
   %return dx, dy
   D = -J\F;
   dx1 = D(1);
   dx2 = D(2);
   
   %update x_1, y_1
   t3_1 = t3_1 + dx1;
   t4_1 = t4_1 + dx2;
   
   if n > 100
       disp ('loop not converging')
       break; 
   end
end

%Extract x_1 and y_1 for solution / convert to radians
t3(jj) = t3_1;
t4(jj) = t4_1;
   
end

figure(1) 
plot(theta2*(180/pi), t3*(180/pi)) 
xlabel('Theta 2 ( degree)')
ylabel('Theta 3 ( degree)') 

figure(2) 
plot(theta2*(180/pi),t4*(180/pi))
xlabel( 'theta 2 (Degrees) ')
ylabel('theta 4 ( degrees)') 

maxM=max(t4*(180/pi)) 
minM=min(t4*(180/pi)) 

Mag=maxM-minM






