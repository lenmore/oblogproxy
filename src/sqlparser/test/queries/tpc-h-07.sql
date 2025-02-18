-- http://www.sqlserver-dba.com/2011/09/this-is-a-followup-on-my-earlier-post-of-sql-server-test-data-generation-testing-tools-i-had-some-requests-for-my-set-up-pr.html
SELECT SUPP_NATION, CUST_NATION, L_YEAR, SUM(VOLUME) AS REVENUE
FROM (SELECT N1.N_NAME                          AS SUPP_NATION,
             N2.N_NAME                          AS CUST_NATION,
             datepart(yy, L_SHIPDATE)           AS L_YEAR,
             L_EXTENDEDPRICE * (1 - L_DISCOUNT) AS VOLUME
      FROM SUPPLIER,
           LINEITEM,
           ORDERS,
           CUSTOMER,
           NATION N1,
           NATION N2
      WHERE S_SUPPKEY = L_SUPPKEY
        AND O_ORDERKEY = L_ORDERKEY
        AND C_CUSTKEY = O_CUSTKEY
        AND S_NATIONKEY = N1.N_NATIONKEY
        AND C_NATIONKEY = N2.N_NATIONKEY
        AND ((N1.N_NAME = 'FRANCE' AND N2.N_NAME = 'GERMANY') OR
             (N1.N_NAME = 'GERMANY' AND N2.N_NAME = 'FRANCE'))
        AND L_SHIPDATE BETWEEN '1995-01-01' AND '1996-12-31') AS SHIPPING
GROUP BY SUPP_NATION, CUST_NATION, L_YEAR
ORDER BY SUPP_NATION, CUST_NATION, L_YEAR