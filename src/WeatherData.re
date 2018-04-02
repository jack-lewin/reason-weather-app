type weather = {
  summary: string,
  temp: float
};

let url = "https://query.yahooapis.com/v1/public/yql?q=select%20item.condition%20from%20weather.forecast%20where%20woeid%20in%20(select%20woeid%20from%20geo.places(1)%20where%20text%3D%22london%22)%20AND%20u%3D%22c%22&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys";

let parseWeatherJson = json =>
  Json.Decode.{
    summary: field("text", string, json),
    temp: float_of_string(field("temp", string, json))
  };

let parseWeatherResultsJson = json =>
  Json.parseOrRaise(json) |> Json.Decode.(at([
    "query",
    "results",
    "channel",
    "item",
    "condition"
  ], parseWeatherJson));

let getWeather = () =>
  Js.Promise.(
    Bs_fetch.fetch(url)
      |> then_(Bs_fetch.Response.text)
      |> then_(
        jsonText => {
          let result = parseWeatherResultsJson(jsonText);
          resolve(result);
        }
      )
);
