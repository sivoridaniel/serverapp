# Application Server

## Running with Docker

Make sure you have [Docker](https://www.docker.com/) installed.

```sh
$ git clone https://github.com/sivoridaniel/serverapp.git # or clone your own fork
$ cd serverapp/AppServer
$ docker build -t app-server .
$ docker run -P app-server
```

Your appserver should now be running on [localhost:3000](http://localhost:3000/).
