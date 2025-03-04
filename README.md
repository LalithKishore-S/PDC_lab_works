MPI Server CLient
ompi-server --report-uri ompi-uri.txt
mpirun --mca pmix_server_uri file:ompi-uri.txt -np 1 ./server
mpirun --mca pmix_server_uri file:ompi-uri.txt -np 1 ./client
