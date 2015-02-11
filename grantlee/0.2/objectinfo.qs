var ObjectInfo = function(input)
{
    return JSON.stringify(input, null, ' ');
};
ObjectInfo.filterName = "objectinfo";
ObjectInfo.isSafe = true;
Library.addFilter("ObjectInfo");
