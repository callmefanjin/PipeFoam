rho = thermo.rho();

volScalarField rAU(1.0/UEqn.A());
surfaceScalarField rhorAUf("rhorAUf", fvc::interpolate(rho*rAU));
volVectorField HbyA(constrainHbyA(rAU*UEqn.H(), U, p));
surfaceScalarField phid
(
    "phid",
    fvc::interpolate(psi)
   *(
        fvc::flux(HbyA)
      + MRF.zeroFilter(rhorAUf*fvc::ddtCorr(rho, U, phi)/fvc::interpolate(rho))
    )
);

MRF.makeRelative(fvc::interpolate(psi), phid);

// Non-orthogonal pressure corrector loop
while (pimple.correctNonOrthogonal())
{
    // Multiply the whole continuity statement by A so that pEqn.flux() is
    // the physical face mass flow phi*Af. Mesh flux is recovered as flux/Af.
    // fvOptions must be scaled too: C++ evaluates the == RHS first.
    fvScalarMatrix pEqn
    (
        A*fvm::ddt(psi, p)
      + fvm::div(phid*Af, p)
      - fvm::laplacian(rhorAUf*Af, p)
     ==
        A*fvOptions(psi, p, rho.name())
      + (fvc::grad(mdot) & vector(1, 0, 0))
    );

    pEqn.solve();

    if (pimple.finalNonOrthogonalIter())
    {
        phi = pEqn.flux()/Af;
    }
}

#include "myrhoEqn.H"
#include "compressibleContinuityErrs.H"

U = HbyA - rAU*fvc::grad(p);
U.correctBoundaryConditions();
fvOptions.correct(U);
K = 0.5*magSqr(U);
