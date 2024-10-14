-- TPC_H Query 15.1 - Create View for Top Supplier Query
CREATE VIEW REVENUE0 (SUPPLIER_NO, TOTAL_REVENUE) AS
SELECT L_SUPPKEY, SUM(L_EXTENDEDPRICE * (1 - L_DISCOUNT))
FROM LINEITEM
WHERE L_SHIPDATE >= '1996-01-01'
  AND L_SHIPDATE < dateadd(mm, 3, cast('1996-01-01' as datetime))
GROUP BY L_SUPPKEY;


-- TPC_H Query 15.2 - Top Supplier
SELECT S_SUPPKEY, S_NAME, S_ADDRESS, S_PHONE, TOTAL_REVENUE
FROM SUPPLIER,
     REVENUE0
WHERE S_SUPPKEY = SUPPLIER_NO
  AND TOTAL_REVENUE = (SELECT MAX(TOTAL_REVENUE) FROM REVENUE0)
ORDER BY S_SUPPKEY;

-- TPC_H Query 15.3 - Drop View
DROP VIEW REVENUE0;