let visualLength: string => int = [%mel.raw
  {|
(text) => {
  return [...new Intl.Segmenter().segment(text)].length
}
|}
];

let useMessage = () => {
  let (message, setMessage) = React.useState(() => "");
  let remaining =
    React.useMemo1(() => 280 - visualLength(message), [|message|]);
  (message, remaining, setMessage);
};
