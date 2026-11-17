FROM gcc:16.2-trixie

ENV DEBIAN_FRONTEND=noninteractive

# ==============================================================================
# System dependencies
# ==============================================================================
RUN apt-get update && apt-get install -y \
    ninja-build python3 python3-pip python3-venv ccache \
    patchelf desktop-file-utils \
    libxcb-cursor0 valgrind \
    libcurl4-openssl-dev libxkbcommon-dev libgl1-mesa-dev \
    libbrotli-dev libexpat1-dev libinih-dev libssh-dev \
    libxml2-utils zlib1g-dev libjpeg-dev libfreetype-dev \
    libpng-dev libuhd-dev libraw-dev libopencv-core-dev libopencv-imgproc-dev \
    libjxl-dev libheif-dev libpystring-dev libtiff-dev libimath-dev libjpeg-turbo-progs \
    libopenexr-dev \
    libyaml-cpp-dev \
    && rm -rf /var/lib/apt/lists/*

# ==============================================================================
# AppImage packaging tools
# ==============================================================================
RUN mkdir -p /opt/tools && cd /opt/tools && \
    curl -fsSL --proto '=https' --tlsv1.2 -o linuxdeploy-x86_64.AppImage \
      https://github.com/linuxdeploy/linuxdeploy/releases/download/<version>/linuxdeploy-x86_64.AppImage && \
    echo "<sha256_attendu>  linuxdeploy-x86_64.AppImage" | sha256sum -c - && \
    curl -fsSL --proto '=https' --tlsv1.2 -o linuxdeploy-plugin-qt-x86_64.AppImage \
      https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/<version>/linuxdeploy-plugin-qt-x86_64.AppImage && \
    echo "<sha256_attendu>  linuxdeploy-plugin-qt-x86_64.AppImage" | sha256sum -c - && \
    chmod +x linuxdeploy*.AppImage

# ==============================================================================
# Install Python core packages
# ==============================================================================
RUN pip3 install --break-system-packages cmake aqtinstall

# ==============================================================================
# Install Qt framework
# ==============================================================================
RUN aqt install-qt linux desktop 6.10.2 linux_gcc_64 --outputdir /opt/qt -m qtshadertools

# ==============================================================================
# Configuration for CMake and library paths
# ==============================================================================
ENV Qt6_DIR=/opt/qt/6.10.2/gcc_64/lib/cmake/Qt6
ENV CMAKE_PREFIX_PATH="/opt/qt/6.10.2/gcc_64;/opt/exiv2;/opt/oiio"
ENV LD_LIBRARY_PATH="/opt/exiv2/lib:/opt/oiio/lib"
ENV PATH="/opt/qt/6.10.2/gcc_64/bin:${PATH}"

# ==============================================================================
# Build and install Exiv2
# ==============================================================================
RUN git clone --depth 1 --branch v0.28.7 \
      https://github.com/Exiv2/exiv2.git /tmp/exiv2-src && \
    cmake -S /tmp/exiv2-src -B /tmp/exiv2-build -G Ninja \
          -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_INSTALL_PREFIX=/opt/exiv2 \
          -DBUILD_SHARED_LIBS=ON \
          -DEXIV2_BUILD_SAMPLES=OFF \
          -DEXIV2_BUILD_TESTS=OFF && \
    cmake --build /tmp/exiv2-build --parallel $(nproc) && \
    cmake --install /tmp/exiv2-build && \
    rm -rf /tmp/*

# ==============================================================================
# Build and install OpenImageIO
# ==============================================================================
RUN git clone --depth 1 --branch v3.1.14.0 \
      https://github.com/AcademySoftwareFoundation/OpenImageIO.git /tmp/oiio-src && \
    cmake -S /tmp/oiio-src -B /tmp/oiio-build -G Ninja \
          -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_INSTALL_PREFIX=/opt/oiio \
          -DEXIV2_ROOT=/opt/exiv2 \
          -DOpenImageIO_BUILD_MISSING_DEPS=required \
          -DSTOP_ON_WARNING=0 \
          -DOIIO_BUILD_TOOLS=OFF \
          -DOIIO_BUILD_TESTS=OFF \
          -DUSE_PYTHON=OFF && \
    cmake --build /tmp/oiio-build --parallel $(nproc) && \
    cmake --install /tmp/oiio-build && \
    rm -rf /tmp/*

# ==============================================================================
# LLVM/Clang toolchain (clang-tidy, run-clang-tidy)
# ==============================================================================
RUN apt-get update && apt-get install -y --no-install-recommends \
        curl gnupg lsb-release ca-certificates jq \
    && curl -fsSL --proto '=https' --tlsv1.2 -o /tmp/llvm-snapshot.gpg.key https://apt.llvm.org/llvm-snapshot.gpg.key \
    && echo "<sha256_attendu>  /tmp/llvm-snapshot.gpg.key" | sha256sum -c - \
    && gpg --dearmor -o /usr/share/keyrings/llvm-archive-keyring.gpg /tmp/llvm-snapshot.gpg.key \
    && rm -f /tmp/llvm-snapshot.gpg.key \
    && echo "deb [signed-by=/usr/share/keyrings/llvm-archive-keyring.gpg] https://apt.llvm.org/trixie/ llvm-toolchain-trixie-23 main" \
        > /etc/apt/sources.list.d/llvm.list \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        clang-23 clang-tidy-23 clang-tools-23 libclang-rt-23-dev \
    && rm -rf /var/lib/apt/lists/*

RUN GCC_DIR="$(dirname "$(gcc -print-libgcc-file-name)")" \
    && for c in clang clang++; do \
         echo "--gcc-install-dir=${GCC_DIR}" > "/usr/lib/llvm-23/bin/${c}.cfg"; \
       done

WORKDIR /app
