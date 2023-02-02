% чтение букв из алфавита
[alphabet,targets]=prprob_myversion;
[R, Q]	= size(alphabet);
[S2,Q]	= size(targets);
%инициализация нейронной сети
S1 = 10; % количество нейронов скрытого слоя
net = newff(minmax(alphabet),[S1 ...
    S2],{'logsig','logsig'},'traingdx'); % создание нейронной сети
net.LW{2,1} = net.LW{2,1}*0.01;
net.b{2} = net.b{2}*0.01;
%обучение нейронной сети без шума
P = alphabet;
T = targets;
net.performFcn = 'sse'; %задаем функцию оценки функционирования
net.trainParam.goal = 0.1;% допустимая погрешность
net.trainParam.show =20; % Частота вывода результатов на экран
net.trainParam.epochs= 5000; %количество циклов обучения
net.trainParam.mc = 0.95;
[net,tr] = train(net,P,T);% обучение нейронной сети
% обучение нейронной сети на зашумленных данных( нейронная сеть2)
netn = net;
netn.trainParam.goal = 0.6;
netn.trainParam.epochs =300;
T = [targets targets targets targets];
for pass  = 1:10
    P =	[alphabet,	alphabet, ...
        (alphabet + randn(R,Q) *0.1), ...
        (alphabet + randn(R,Q) *0.2) ];
    [netn,tr] = train (netn, P,T);
end;

% повторное обучение без шума нейронной сети 1
netn.trainParam.goal = 0.1;
netn.trainParam.epochs = 500;
net.trainParam.show = 5;
[netn,tr] = train(netn,P,T);
%разбиение фамилии на буквы
second_name = 'анисимов';
c = char(second_name);% получение номеров букв в талице юникод
k = unique(double(c)- 1071);% получение уникальных(не повторяющихся )индексов букв в ал- фавите
disp(k);
% тестирование нейронной сети при различных значениях отклонения шума
noise_range = 0:.05:.5;
max_test = 100; % количество тестов
network1 = [];
network2 = [];
T = targets;
for noiselevel = noise_range
    errors1=0;
    errors2=0;
    for i=1:max_test
        P = alphabet + randn(35,32)*noiselevel; % зашумление буквы
        A = sim(net,P);	%тестирование нейронной сетью 1
        AA = compet(A);% овтет нейронной сети - индекс буквыв алфавите
        errors1 = errors1 + sum(sum(abs(AA - T)))/2; % вычисленеи среднего значения ошибки
        An = sim(netn,P);	% идентичная процедура для нейронки 2
        AAn = compet(An);
        errors2 = errors2 + sum(sum(abs(AAn - T)))/2;
    end
    network1 = [network1 errors1/33/100]; %первой сети
    network2 = [network2 errors2/33/100]; %второй сети
end
title('Погрешность сети в зависимости от уровня входного шума '); %
plot(noise_range,network1*100,noise_range,network2*100); 
legend('без шума','с шумом')
for i = 1:length(k)
    letter_pos = k(i);
    noisy_letter = alphabet(:,letter_pos)+randn(35,1)*0.2;% зашумляем букву
    figure;
    plotchar(noisy_letter); % выводим ббукву

    A2 = sim(net,noisy_letter); % передаем зашумленную букву нейронке
    A2 = compet(A2); % округляем результаты нейронки
    answer = find(compet(A2));	% получаем позицию ответа нейронки, индекс найденного нейронкой символа
    figure;
    plotchar(alphabet(:,answer));% выводим символ
end
