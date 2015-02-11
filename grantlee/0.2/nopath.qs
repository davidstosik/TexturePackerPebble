var NoPathFilter = function(input)
{
  var input = input.rawString();
  var tokens = input.split('/');
  return tokens[tokens.length - 1];
};

NoPathFilter.filterName = "nopath";

Library.addFilter("NoPathFilter");


