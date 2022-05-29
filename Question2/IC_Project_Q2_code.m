clear all; % clears the workspace everytime we run the code  
clc;

n = 20; % the length of the code
k = 10; % the factor used to define the number of random vectors chosen from the total number of vectors (2^n).
p = 0.1; % the probability of  
E = 0; % Variable to count the number of errors in a codewords
N = 1000; % number of iterations

Code_size = 2^k; % the number of random vectors we choose from the binary vector space {0,1}^n.
vec_space = 2^n; % the total number of vectors 

P_error = zeros(1,5); % declares the probability of error as an array of 5 zeros as we repeat the exercise 5 times

for m = 1:5 % for loop for finding the probability of error 5 times by using different codes for the same values of n,k,p 
    %% Get 2^k numbers in range of 0 to 2^n -1
    Code = randi(vec_space, [1, Code_size]) - 1; % finds 2^k random values between 0 and (2^n-1).
    
    %% Then use de2bi to convert each number to binary. This is our Code C
    C = de2bi(Code); % converts the chosen 2^k vectors to binary strings of n length.

    E = 0; % declares E = 0 for each different code
    for k = 1:N 
        %% Pick random index and that is our transmitted codeword
        index = randi(Code_size); % chooses a random codeword from the 2^k vectors consisting of the code
        Codeword = C(index, :); 
        
        original = Codeword;
        r = rand(1,n); % finds n values between 0 to 1 in the form of a 1xn matrix.
        %% refer to above link for flipping bits of codeword to obtain y
        for i = 1:n % in this for loop, we check if each value of r is greater than or lesser than the "bit flip probability". 
            if(r(i) <= p) % if the value of r(i) ≤ p then we flip the codeword (change bit from 0 to 1 and vice versa)
                if(Codeword(i) == 0)
                    Codeword(i) = 1;
                else
                    Codeword(i) = 0;
                end
            end
        end
        y = Codeword; % declares y as the received codeword.
        % this gives flipped codeword and ultimately our y vector
        %% Use min dist decoder, so compare y with all codewords possible 
        
        dist = zeros(1, Code_size); % declares the array 'dist' to find the distance between the received codeword and transmitted codeword.
        for i = 1:Code_size
            dist(i) = nnz(y - C(i, :)); % calculates the number of non-zero values of each codeword from (y-C) 
        end
        
        [argmin, index_of_min] = min(dist); % returns the minimum value of the distance and the index of the codeword in C
        estimate = C(index_of_min, :); % declares our estimate as C(index_of_min, ;)
        
        %% Indicator value - 1 if error, 0 otherwise
        
        if (nnz(estimate - original) == 0) % if the codeword was decoded perfectly then indicator_val = 0, else it is 1
            Indicator_val = 0;
        else
            Indicator_val = 1;
        end
        
        %% E = E + I
        E = E + Indicator_val;
        
    end
    
    %% get the probability of error
    P_error(m) = E/N;
end

disp('The error probabilities are as follows :');
%% Show outputs for 5 different C for each given value
disp(P_error);

disp('The minimum probability of error is :');
%% Among these 5, get the minimum and we let it be the value-tuple
min_error = min(P_error);

disp(min_error);