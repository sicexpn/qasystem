$(document).ready(function () {
    $("#p1_childs").hide();
    $("#p2_childs").hide();
    $("#p1").click(function () {
        $("#p1_childs").toggle();
    });
    $("#p2").click(function () {
        $("#p2_childs").toggle();
    });
});