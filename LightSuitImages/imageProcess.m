
I = imread("G.bmp");
output = "{";

for j = 1:16
output = strcat(output,"{");
	for a=1:2
	output = strcat(output,"B");
	if a == 1 
		for i = (1:8)
			output = strcat(output,int2str(I(j,i)));
		end
		
			output = strcat(output,",");
		
	else
		for i = (9:16)
			output = strcat(output,int2str(I(j,i)));
		end
	end
end
	output = strcat(output,"},");
end
fid = fopen('output.txt','w');
fprintf(fid,'%s',output);
fclose(fid);
