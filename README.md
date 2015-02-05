LSBF -- Locality-Sensitive Bloom Filter for Approximate Membership Query
========================================================================

This is a simple implemention of LSBF.

More information and the design details can be found in the following paper:

  * [Yu Hua](http://stlab.wnlo.hust.edu.cn/csyhua/index.html), Bin Xiao, Bharadwaj Veeravalli, Dan Feng. "Locality-Sensitive Bloom Filter for Approximate Membership Query", IEEE Transactions on Computers (TC), Vol. 61, No. 6, June 2012, pages: 817-830. [(pdf)](http://cs.hust.edu.cn/stlab/csyhua/hua-tc2012.pdf)



About Files
-----------

`lsbf.c`        -- the main function of approximate query

`exact_query.c` -- the main function of exact-matching query

`dataset/`      -- include the test data files


How to use
----------

./exact_query 200 200 2 dataset/200_data dataset/200_query
> The exact query result are 28, 96, 98. These three points meet the needs of query requests.

./lsbf 200 200 2 dataset/200_data dataset/200_query








