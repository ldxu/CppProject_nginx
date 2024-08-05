# 使用官方的 Ubuntu 作为基础镜像
FROM ubuntu:20.04

# 设置环境变量，防止交互式安装界面
ENV DEBIAN_FRONTEND=noninteractive

# 更新包列表并安装必要的软件包
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    curl \
    vim \
    libmysqlclient-dev \
    libhiredis-dev \
    libconfig++-dev \
    libspdlog-dev \
    libpthread-stubs0-dev \
    && apt-get clean && \
    rm -rf /var/lib/apt/lists/*


# 设置工作目录
WORKDIR /usr/src/my_project


