FROM agulinari/appserver:v2
COPY AppServer /AppServer
COPY log4cpp.properties /
RUN cd /AppServer/cmake && cmake . && make
EXPOSE 3000
CMD ["./AppServer/cmake/AppServer", "-D", "FOREGROUND"]
