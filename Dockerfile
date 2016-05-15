FROM agulinari/appserver
COPY AppServer /AppServer
RUN ls
RUN cd /AppServer/cmake && cmake . && make
EXPOSE 3000
CMD ["./AppServer/cmake/AppServer", "-D", "FOREGROUND"]