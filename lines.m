baseX = cumsum([0, 1.372, 4.115, 4.115, 1.372]);
baseY = cumsum([0, 5.486, 6.401]);


outerLines =[baseX(1), baseY(end);
    baseX(1), baseY(1);
    baseX(end), baseY(1)
    baseX(end), baseY(end);];

innerLines =[baseX(2), baseY(end);
    baseX(2), baseY(1);
    baseX(4), baseY(1)
    baseX(4), baseY(end);];

serveHorizontal= [baseX(2), baseY(2);
    baseX(4), baseY(2)];

serveVertical= [baseX(3), baseY(2);
    baseX(3), baseY(3)];

baseMid = [baseX(3), 0.05*(baseY(2) - baseY(1));
    baseX(3), baseY(1)];

figure(1)
clf
hold on
plot(outerLines(:,1), outerLines(:,2), 'k-')
plot(innerLines(:,1), innerLines(:,2), 'k-')
plot(serveHorizontal(:,1), serveHorizontal(:,2), 'k-')
plot(serveVertical(:,1), serveVertical(:,2), 'k-')
plot(baseMid(:,1), baseMid(:,2), 'k-')
axis([-1, 13, -1, 13])

