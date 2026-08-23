volScalarField& e = thermo.he();
const volScalarField& psi = thermo.psi();
volScalarField ee
(
    IOobject
    (
        "ee",
        runTime.timeName(),
        mesh,
        IOobject::NO_READ,
        IOobject::AUTO_WRITE
    ),
    e
);

