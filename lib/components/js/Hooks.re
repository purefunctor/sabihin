let useMemoPrevious = (makeInitial, compute, dependencies) => {
  let (previousValue, setPreviousValue) = React.useState(makeInitial);
  React.useEffect1(
    () => {
      setPreviousValue(previousValue => compute(previousValue));
      None;
    },
    dependencies,
  );
  previousValue;
};
