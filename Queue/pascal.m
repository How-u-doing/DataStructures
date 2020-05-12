% get the row vector of the coefficents of (a+b)^n
function u2 = pascal(n)
u1=ones(1,2);
u2=u1;
for i=2:n
    u2=ones(1,i+1);
    for j=2:i
        u2(j)=u1(j-1)+u1(j);
    end
    u1=u2;
end
end

