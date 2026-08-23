{
    fvScalarMatrix myrhoEqn
    (
        fvm::ddt(rho)
      + fvc::div(phi*Af)/A
      ==
        fvOptions(rho)
      + (fvc::grad(mdot) & vector(1, 0, 0))/A
    );

    fvOptions.constrain(myrhoEqn);

    myrhoEqn.solve();

    fvOptions.correct(rho);
}

// ************************************************************************* //
