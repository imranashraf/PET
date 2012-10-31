PET: Partition Evaluation Tool
===============================

This repository contains our tool which can perform partition evaluation of various partitioning algorithms.
Partitioning algorithms can be implemented as modules in our tool. In order to perform evaluation cost metrics
are calculated depending upon the load balancing, coupling degree of function in a cluster and inter-cluster
communication. Random applications following some realistic profiles can be generated, which are used to
provide input test applications to the partitioning algorithms for perform evaluation. Furthermore, an
exhaustive search algorthm is also implemented to find the optimal solutions for smaller applications, to
compare a certain algorithm against the optimal partition.

