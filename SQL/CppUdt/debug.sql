-- create a variable of the type, create a value of the type and invoke
-- a behavior over it

declare @c CppComplexNumber
set @c = convert(CppComplexNumber, '(1,2i)')
select @c.ToString()

