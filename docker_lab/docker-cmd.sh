
USER=`whoami`
USER_ID=`id -u ${USER}`
USER_GROUP=`id -g ${USER}`
# Build the docker image by Dockerfile
docker build --no-cache -t dutsai:test .
docker build --no-cache -t dutsai:debug .
# Run bash command in the docker container, exit when done
docker run --rm --name dutsai-run -v /home/dutsai:/dutsai dutsai:debug bash -c "ls /dutsai"
# Run with user name permission
docker run --rm -u $USER_ID:$USER_GROUP --name dutsai-run -v /home/dutsai:/dutsai dutsai:debug bash -c "ls /dutsai"


# Run the docker in the background
# --rm Automatically remove the container when it exits
# -t           bind a tty std output
# -d           detache mode
# --name       container name
# -v          host folder
# dutsai:debug  image name
docker run --rm -t -d --name dutsai-run -v /home/dutsai:/dutsai dutsai:debug
docker run --rm -t -d --name dutsai-run dutsai:debug
# run bash command when docker container is running
docker exec -i dutsai-run ls "$@"
docker exec -it dutsai-run bash
