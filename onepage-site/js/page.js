var app = angular.module('akbatra', [
'ngRoute'
]);


app.config(['$routeProvider', function ($routeProvider) {
    $routeProvider
        .when("/", {templateUrl: "templates/home.html", controller: "PageCtrl"})
        .when("/about", {templateUrl: "templates/about.html", controller: "PageCtrl"})
        .when("/coding", {templateUrl: "templates/coding.html", controller: "PageCtrl"})
        .when("/imagery", {templateUrl: "templates/imagery.html", controller: "PageCtrl"})
        .when("/404", {templateUrl: "templates/error.html", controller: "PageCtrl"})
        .otherwise({redirectTo: '/404'});
}]);

app.controller('PageCtrl', function (/* $scope, $location, $http */) {

});
