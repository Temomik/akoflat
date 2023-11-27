window.onload = function(){
    slideOne();
    slideTwo();
    toggleMainButton(true);

    Telegram.WebApp.MainButton
    .setText('Submit')
    .show()
    .onClick(function () {
        OnSubmit();
    });
}

$('body').css('visibility', '');

let sliderOne = document.getElementById("slider-1");
let sliderTwo = document.getElementById("slider-2");
let displayValOne = document.getElementById("range1");
let displayValTwo = document.getElementById("range2");
let minGap = 10;
let sliderTrack = document.querySelector(".slider-track");
let sliderMaxValue = document.getElementById("slider-1").max;

function slideOne(){
    if(parseInt(sliderTwo.value) - parseInt(sliderOne.value) <= minGap){
        sliderOne.value = parseInt(sliderTwo.value) - minGap;
    }
    displayValOne.textContent = sliderOne.value + "$";
    fillColor();
}
function slideTwo(){
    if(parseInt(sliderTwo.value) - parseInt(sliderOne.value) <= minGap){
        sliderTwo.value = parseInt(sliderOne.value) + minGap;
    }
    displayValTwo.textContent = sliderTwo.value + "$";
    fillColor();
}
function fillColor() {
    percent1 = (sliderOne.value / sliderMaxValue) * 100;
    percent2 = (sliderTwo.value / sliderMaxValue) * 100;
    sliderTrack.style.background = `linear-gradient(to right, #dadae5 ${percent1}% , #3264fe ${percent1}% , #3264fe ${percent2}%, #dadae5 ${percent2}%)`;
}

var flatCounts = []
function toggleFlatCheckboxArea(element) {
    var text = element.textContent;
    flatCounts = toggleMultiCheckbox(text, flatCounts, element);
}

var city = ""
function toggleCityCheckboxArea(element) {
    var text = element.textContent;
    city = toggleCheckbox(text, city, element);
    console.log(city);
}

var platforms = []
function togglePlatformsCheckboxArea(element) {
    var text = element.textContent;
    platforms = toggleMultiCheckbox(text, platforms, element);
}

function toggleMultiCheckbox(text, container, element) {
    if (container.find((item) => item == text))
    {
        element.style.background = "#548cc5";

        const index = container.indexOf(text);
        if (index > -1) { // only splice array when item is found
            container.splice(index, 1); // 2nd parameter means remove one item only
            console.log("removed" + text)
        }

        return container;
    }

    container.push(text);
    element.style.background = "#3264fe";

    return container;
}

function toggleCheckbox(text, prevValue, element) {
    if (text == prevValue)
    {
        return text;
    }

    var childs = element.parentNode.querySelectorAll('li');

    childs.forEach((child) => child.style.background = "#548cc5");
    element.style.background = "#3264fe";

    return text;
}

// enum command
// {
//     None = 0,
//     SetUserConfig,
//     Start,
//     Stop
// }

function GetJsonObject() {

    commandJson = {}
    commandJson ["minPrice"] = parseInt(sliderOne.value);
    commandJson ["maxPrice"] = parseInt(sliderTwo.value);

    var flatCountInt = flatCounts.map(function (x) { 
        return parseInt(x); 
      });

    commandJson ["city"] = city;
    commandJson ["floorCount"] = flatCountInt
    commandJson ["platforms"] = platforms;


    jsonObject = {}

    jsonObject ["command"] = 1;
    jsonObject ["value"] = commandJson;

    return jsonObject;
}

function IsDataValid()
{
    return city.length != "" && flatCounts.length != 0 && platforms.length != 0;
}

function toggleMainButton(isForceShow = false) {
    var mainButton = Telegram.WebApp.MainButton;
    if (!mainButton.isVisible || isForceShow) {
        mainButton.show();
    } else {
        mainButton.hide();
    }
}

function OnSubmit() {
    if (!IsDataValid())
    {
        alert("Make sure all fields are configured!");
        return;
    }

    var id = Telegram.WebApp.id
    var data = JSON.stringify(GetJsonObject());

    console.log(data);

    Telegram.WebApp.sendData(data);
}

var initData = Telegram.WebApp.initData || '';
var initDataUnsafe = Telegram.WebApp.initDataUnsafe || {};

function sendMessage(msg_id, with_webview) {
    if (!initDataUnsafe.query_id) {
    alert('WebViewQueryId not defined');
    return;
    }
    $('button').prop('disabled', true);
    $('#btn_status').text('Sending...').removeClass('ok err').show();
    $.ajax('/demo/sendMessage', {
        type: 'POST',
        data: {
            _auth: initData,
            msg_id: msg_id || '',
            with_webview: !initDataUnsafe.receiver && with_webview ? 1 : 0
        },
        dataType: 'json',
        success: function (result) {
            $('button').prop('disabled', false);
            if (result.response) {
                if (result.response.ok) {
                    $('#btn_status').html('Message sent successfully!').addClass('ok').show();
                } else {
                    $('#btn_status').text(result.response.description).addClass('err').show();
                    alert(result.response.description);
                }
            } else {
                $('#btn_status').text('Unknown error').addClass('err').show();
                alert('Unknown error');
            }
        },
        error: function (xhr) {
            $('button').prop('disabled', false);
            $('#btn_status').text('Server error').addClass('err').show();
            alert('Server error');
        }
    });
}
