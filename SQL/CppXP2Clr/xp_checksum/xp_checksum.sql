-- This script updates a table and calls an extended
-- procedure which retrieves rows from the same table.
-- Its purpose is to show how two connections can work with 
-- the same rows, without blocking each other. This is possible 
-- because the connection opened by the extended stored 
-- procedure xp_checksum is bound to the connection that
-- the procedure is called from.
-- 
-- Before running xp_checksum, copy xp_checksum.dll to
-- ..\Microsoft SQL Server\MSSQL.1\MSSQL\Binn folder
                                 
use master
go

sp_addextendedproc 'xp_checksum', 'xp_checksum.dll'
go

begin transaction
exec xp_checksum '[AdventureWorks].[Person].[Address]'
commit transaction
go

sp_dropextendedproc 'xp_checksum'
go

dbcc xp_checksum(free)
go