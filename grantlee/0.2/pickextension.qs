var PickExtensionFilter = function(input)
{
  var input = input.rawString();
  var tokens = input.split('.');
  return tokens[tokens.length - 1];
};

PickExtensionFilter.filterName = "pickextension";

Library.addFilter("PickExtensionFilter");

