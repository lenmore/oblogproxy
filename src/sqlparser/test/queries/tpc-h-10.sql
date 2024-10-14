-- http://www.sqlserver-dba.com/2011/09/this-is-a-followup-on-my-earlier-post-of-sql-server-test-data-generation-testing-tools-i-had-some-requests-for-my-set-up-pr.html
SELECT TOP 20 C_CUSTKEY, C_NAME,
       SUM(L_EXTENDEDPRICE * (1 - L_DISCOUNT)) AS REVENUE,
       C_ACCTBAL,
       N_NAME,
       C_ADDRESS,
       C_PHONE,
       C_COMMENT
FROM CUSTOMER,
     ORDERS,
     LINEITEM,
     NATION
WHERE C_CUSTKEY = O_CUSTKEY
  AND L_ORDERKEY = O_ORDERKEY
  AND O_ORDERDATE >= '1993-10-01'
  AND O_ORDERDATE < dateadd(mm, 3, cast('1993-10-01' as datetime))
  AND L_RETURNFLAG = 'R'
  AND C_NATIONKEY = N_NATIONKEY
GROUP BY C_CUSTKEY, C_NAME, C_ACCTBAL, C_PHONE, N_NAME, C_ADDRESS, C_COMMENT
ORDER BY REVENUE DESC