FROM scratch
COPY --from=dockcross/linux-x64 /usr/lib/x86_64-linux-gnu/libstdc++.so.6 /usr/lib/x86_64-linux-gnu/libstdc++.so.6
COPY --from=dockcross/linux-x64 \
        /lib/x86_64-linux-gnu/libm.so.6 \
        /lib/x86_64-linux-gnu/libgcc_s.so.1 \
        /lib/x86_64-linux-gnu/libc.so.6 \
        /lib/x86_64-linux-gnu/libnss_files.so.2 \
        /lib/x86_64-linux-gnu/
COPY --from=dockcross/linux-x64 /lib64/ld-linux-x86-64.so.2 /lib64/
ADD SmallBinary /
ENTRYPOINT ["/SmallBinary"]
